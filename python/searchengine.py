#!/usr/bin/python
import urllib2
from BeautifulSoup import *
from urlparse import urljoin
from pysqlite2 import dbapi2 as sqlite

ignorewords = set(['the', 'of', 'to', 'and', 'a', 'in', 'is', 'it'])

# db schema
"""
link (rowid, formid, toid), stores two URL IDs, indicating a link from one table to another
urllist (rowid, url)
wordlocation (urlid, wordid, location), a list of the locations of words in the documents
wordlist (rowid, word)
linkwords (wordid, linkid), uses the wordid and linkid columns to store which words are actually used in that link
"""

class crawler:
  def __init__(self, dbname):
    self.con = sqlite.connect(dbname)

  def __del__(self):
    self.con.close()

  def dbcommit(self):
    self.con.commit()

  def getentryid(self, table, field, value, createnew=True):
    cur = self.con.execute(
        "select rowid from %s where %s = '%s'" % (table, field, value))
    res = cur.fetchone()
    if res == None:
      cur = self.con.execute(
          "insert into %s (%s) values ('%s')" % (table, field, value))
      return cur.lastrowid
    else:
      return res[0]

  def addto_index(self, url, soup):
    if self.isindexed(url): return
    print 'Indexing %s' % url

    text = self.gettextonly(soup)
    words = self.separate_words(text)

    urlid = self.getentryid('urllist', 'url', url)

    for i in range(len(words)):
      word = words[i]
      if word in ignorewords: continue
      wordid = self.getentryid('wordlist', 'word', word)
      sql = "insert into wordlocation(urlid,wordid,location) \
          values (%d,%d,%d)" % (urlid, wordid, i)
      #print sql
      self.con.execute(sql)

  def gettextonly(self, soup):
    v = soup.string
    if v == None:
      c = soup.contents
      result_text = ''
      for t in c:
        sub_text = self.gettextonly(t)
        result_text += sub_text + '\n'
      return result_text.strip()
    else:
      return v.strip()
  
  def separate_words(self, text):
    splitter = re.compile('\\W*')
    return [s.lower() for s in splitter.split(text) if s != '']

  def isindexed(self, url):
    u = self.con.execute(
        "select rowid from urllist where url = '%s'" % url).fetchone()
    if u != None:
      v = self.con.execute(
          'select * from wordlocation where urlid = %d' % u[0]).fetchone()
      if v != None: return True
    return False

  def addlinkref(self, urlFrom, urlTo, linktext):
    pass

  def crawl(self, pages, depth=2):
    for i in range(depth):
      newpages = set()
      for page in pages:
        try:
          page_content = urllib2.urlopen(page).read()
        except:
          print 'could not open %s' % page
          continue
        soup = BeautifulSoup(page_content)
        self.addto_index(page, soup)

        links = soup('a')
        for link in links:
          if ('href' in dict(link.attrs)):
            url = urljoin(page, link['href'])
            if url.find("'") != -1: continue
            url = url.split('#')[0]
            if url[0:4] == 'http' and not self.isindexed(url):
              newpages.add(url)
            linkText = self.gettextonly(link)
            self.addlinkref(page, url, linkText)
        self.dbcommit()
      pages = newpages

  def createindextables(self):
    self.con.execute('create table urllist(url)')
    self.con.execute('create table wordlist(word)')
    self.con.execute('create table wordlocation(urlid,wordid,location)')
    self.con.execute('create table link(fromid integer,toid integer)')
    self.con.execute('create table linkwords(wordid,linkid)')
    self.con.execute('create index wordidx on wordlist(word)')
    self.con.execute('create index urlidx on urllist(url)')
    self.con.execute('create index wordurlidx on wordlocation(wordid)')
    self.con.execute('create index urltoidx on link(toid)')
    self.con.execute('create index urlfromidx on link(fromid)')
    self.dbcommit( )


class searcher:
  def __init__(self, dbname):
    self.con = sqlite.connect(dbname)

  def __del__(self):
    self.con.close()

  def get_all_words(self):
    sql = "select * from wordlist"
    rows = self.con.execute(sql).fetchall()
    return [row for row in rows]
    
  def location_score(self, rows):
    locations = dict([(row[0], 1000000) for row in rows])
    for row in rows:
      loc = sum(row[1:])
      if loc < locations[row[0]]: locations[row[0]] = loc

    return self.normalize_scores(locations, small_is_better = 1)

  def distance_score(self, rows):
    # if there's only one word, everyone wins
    if len(rows[0]) <= 2: return dict([(row[0], 1.0) for row in rows])

    # initialize the dictionary with large values
    min_distance = dict([(row[0], 1000000) for row in rows])

    for row in rows:
      dist = sum([abs(row[i] - row[i-1]) for i in range(2, len(row))])
      if dist < min_distance[row[0]]: min_distance[row[0]] = dist
    return self.normalize_scores(min_distance, small_is_better = 1)

  def normalize_scores(self, scores, small_is_better = 0):
    vsmall = 0.00001 # avoid division by zero errors
    if small_is_better:
      min_score = min(scores.values())
      return dict([(k, float(min_score)/max(vsmall, v)) for (k, v) \
          in scores.items()])
    else:
      max_score = max(scores.values())
      if max_score == 0: max_score = vsmall
      return dict([(k, float(v)/max_score) for (k, v) \
          in scores.items()])

  def frequency_score(self, rows):
    counts = dict([(row[0], 0) for row in rows])
    for row in rows: counts[row[0]] += 1
    return self.normalize_scores(counts, small_is_better = 1)

  def get_scored_list(self, rows, word_ids):
    """
    rows: [(urlid, location1, location2, ...), (urlid, location1, location2, ...), ...]
    eg:
    [(24, 69, 226),
     (24, 69, 232),
     (24, 69, 291),
     (24, 73, 137),
     (24, 73, 226),
     (25, 73, 232),
     (25, 73, 291),
     (25, 84, 137),
     (25, 84, 226)]
    """
    total_scores = dict([(row[0], 0) for row in rows])

    # weights = [(1.0, self.frequency_score(rows))]
    # weights = [(1.0, self.location_score(rows))]
    weights = [(1.0, self.distance_score(rows))]

    for (weight, scores) in weights:
      for url in total_scores:
        total_scores[url] += weight * scores[url]

    return total_scores

  def get_url_name(self, id):
    return self.con.execute(
        "select url from urllist where rowid = %d" % id).fetchone()[0]

  def query(self, q):
    rows, word_ids = self.get_matchrows(q)
    scores = self.get_scored_list(rows, word_ids)
    rankedscores = sorted([(score, url) for (url, score) in scores.items()], reverse = 1)
    for (score, urlid) in rankedscores[0:10]:
      print '%f\t%s' % (score, self.get_url_name(urlid))

  def get_matchrows(self, q):
    # strings to build the query
    field_list = 'w0.urlid'
    table_list = ''
    clause_list = ''
    word_ids = []

    # split the words by spaces
    words = q.split(' ')
    table_number = 0

    for word in words:
      # get the word id
      sql = "select rowid from wordlist where word = '%s'" % word
      word_row = self.con.execute(sql).fetchone()
      if word_row != None:
        word_id = word_row[0]
        word_ids.append(word_id)
        if table_number > 0:
          table_list += ','
          clause_list += ' and '
          clause_list += 'w%d.urlid=w%d.urlid and ' % (table_number - 1, table_number)
        field_list += ',w%d.location' % table_number
        table_list += 'wordlocation w%d' % table_number
        clause_list += 'w%d.wordid=%d' % (table_number, word_id)
        table_number += 1

    # create the query from the separate parts
    full_query = 'select %s from %s where %s' % (field_list, table_list, clause_list)
    # print 'full query: [%s]' % full_query
    cur = self.con.execute(full_query)
    rows = [row for row in cur]
    #print rows[1:10]

    return rows, word_ids
