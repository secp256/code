#!/usr/bin/env python
#-*- coding: UTF-8 -*-

import sys
import re
import threading
import signal
import json
import time
import io
import os
import pycurl
import urlparse
import StringIO
import gzip
import logging
import logger
import uuid
import urllib
#import pika
import config
from SocketServer import ThreadingTCPServer, StreamRequestHandler
from pyquery import PyQuery as pq
from multiprocessing.dummy import Pool as ThreadPool
# from logging.handlers import RotatingFileHandler
# from multiprocessing.util import get_logger, DEFAULT_LOGGING_FORMAT


# format = logging.Formatter(DEFAULT_LOGGING_FORMAT)
# normal_hand = RotatingFileHandler('log/multiprocessing.log')
# normal_hand.setLevel(logging.DEBUG)
# normal_hand.setFormatter(format)
# logger = get_logger()
# #logger.propagate = 1
# logger.setLevel(logging.DEBUG)
# logger.addHandler(normal_hand)


reload(sys)
sys.setdefaultencoding('utf-8')
logger = logging.getLogger('task')


def gzdecode(data):
    compressedstream = StringIO.StringIO(data)
    gziper = gzip.GzipFile(fileobj=compressedstream)

    try:
        data2 = gziper.read()
    except:
        return data

    return data2


class MyCurl(object):

    def __init__(self, **param):
        self.buf = io.BytesIO()

        share = pycurl.CurlShare()
        #多个Curl可以共享cookie
        share.setopt(pycurl.SH_SHARE, pycurl.LOCK_DATA_COOKIE)
        share.setopt(pycurl.SH_SHARE, pycurl.LOCK_DATA_DNS)

        post_c = pycurl.Curl()
        post_c.setopt(pycurl.SHARE, share)
        post_c = self._init(post_c, param)
        post_c.setopt(pycurl.WRITEFUNCTION, self.buf.write)
        self.post_c = post_c

        get_c = pycurl.Curl()
        get_c.setopt(pycurl.SHARE, share)
        get_c = self._init(get_c, param)
        get_c.setopt(pycurl.WRITEFUNCTION, self.buf.write)
        self.get_c = get_c

        down_c = pycurl.Curl()
        down_c.setopt(pycurl.SHARE, share)
        down_c = self._init(down_c, param)
        down_c.setopt(pycurl.WRITEFUNCTION, self.buf.write)
        self.down_c = down_c

        upload_c = pycurl.Curl()
        upload_c.setopt(pycurl.SHARE, share)
        upload_c = self._init(upload_c, param)
        upload_c.setopt(pycurl.WRITEFUNCTION, self.buf.write)
        upload_c.setopt(pycurl.TIMEOUT, 300)
        self.upload_c = upload_c

        self.param = param
        self.share = share

    def __del__(self):
        self.get_c.close()
        self.upload_c.close()
        self.down_c.close()
        self.post_c.close()
        self.buf.close()





    def get_page(self, url, method='get', post_data=None, cookie=None, referer=None, header=''):
        url = url.encode('utf-8')

        self.buf.seek(0)
        self.buf.truncate()
        if method == 'post':
            c = self.post_c
            c.setopt(pycurl.POSTFIELDS, post_data)
        else:
            c = self.get_c

        if cookie:
            c.setopt(pycurl.COOKIE, cookie)

        _referer = ''
        for l in self.head_list:
            name = l.split(':', 1)[0]
            if name == 'Referer':
                _referer = l
        if _referer:
            self.head_list.remove(_referer)
        if referer:
            #c.setopt(pycurl.REFERER, referer)
            self.head_list.append('Referer: ' + referer)
            c.setopt(pycurl.HTTPHEADER, self.head_list)
        else:
            c.setopt(pycurl.HTTPHEADER, self.head_list)

        if header:
            _header = list(self.head_list)
            for l in header:
                name = l.split(':', 1)[0]
                for j in self.head_list:
                    _name = j.split(':', 1)[0]
                    if name == _name:
                        _header.remove(j)
                _header.append(l)

            c.setopt(pycurl.HTTPHEADER, _header)

        c.setopt(pycurl.URL, url)
        #get_c.setopt(pycurl.PROXY, 'http://127.0.0.1:8888')
        c.perform()

        m = self.info(c)
        v = self.buf.getvalue()

        return m, v

    def down_file(self, url, file_path, mode='wb+'):

        f = open(file_path, mode)
        self.down_c.setopt(pycurl.URL, url)
        #win下不能用
        #self.down_c.setopt(pycurl.WRITEDATA, file_path)
        self.down_c.setopt(pycurl.WRITEFUNCTION, f.write)
        #self.down_c.setopt(pycurl.PROXY, 'http://127.0.0.1:8888')
        self.down_c.perform()

        f.flush()
        f.close()

    def upload_file(self, url, file, post_data=[]):

        self.buf.seek(0)
        self.buf.truncate()
        self.upload_c.setopt(pycurl.URL, url)
        self.upload_c.setopt(pycurl.POST, 1)
        self.upload_c.setopt(pycurl.WRITEFUNCTION, self.buf.write)
        allfile = []
        for l in post_data:
            allfile.append((l[0], (pycurl.FORM_CONTENTS, l[1])))
        for f in file:
            allfile.append((f[0], (pycurl.FORM_FILENAME, f[1], pycurl.FORM_FILE, f[2])))
        #self.upload_c.setopt(pycurl.PROXY, 'http://127.0.0.1:8888')
        self.upload_c.setopt(pycurl.HTTPPOST, allfile)
        self.upload_c.perform()

        m = self.info(self.upload_c)
        v = self.buf.getvalue()
        return m, v

    def info(self, handle):
        m = {}
        m['effective-url'] = handle.getinfo(pycurl.EFFECTIVE_URL)
        m['http-code'] = handle.getinfo(pycurl.HTTP_CODE)
        m['total-time'] = handle.getinfo(pycurl.TOTAL_TIME)
        m['namelookup-time'] = handle.getinfo(pycurl.NAMELOOKUP_TIME)
        m['connect-time'] = handle.getinfo(pycurl.CONNECT_TIME)
        m['pretransfer-time'] = handle.getinfo(pycurl.PRETRANSFER_TIME)
        m['redirect-time'] = handle.getinfo(pycurl.REDIRECT_TIME)
        m['redirect-count'] = handle.getinfo(pycurl.REDIRECT_COUNT)
        m['size-upload'] = handle.getinfo(pycurl.SIZE_UPLOAD)
        m['size-download'] = handle.getinfo(pycurl.SIZE_DOWNLOAD)
        m['speed-upload'] = handle.getinfo(pycurl.SPEED_UPLOAD)
        m['header-size'] = handle.getinfo(pycurl.HEADER_SIZE)
        m['request-size'] = handle.getinfo(pycurl.REQUEST_SIZE)
        m['content-length-download'] = handle.getinfo(pycurl.CONTENT_LENGTH_DOWNLOAD)
        m['content-length-upload'] = handle.getinfo(pycurl.CONTENT_LENGTH_UPLOAD)
        m['content-type'] = handle.getinfo(pycurl.CONTENT_TYPE)
        m['response-code'] = handle.getinfo(pycurl.RESPONSE_CODE)
        m['speed-download'] = handle.getinfo(pycurl.SPEED_DOWNLOAD)
        m['ssl-verifyresult'] = handle.getinfo(pycurl.SSL_VERIFYRESULT)
        m['filetime'] = handle.getinfo(pycurl.INFO_FILETIME)
        m['starttransfer-time'] = handle.getinfo(pycurl.STARTTRANSFER_TIME)
        m['redirect-time'] = handle.getinfo(pycurl.REDIRECT_TIME)
        m['redirect-count'] = handle.getinfo(pycurl.REDIRECT_COUNT)
        m['http-connectcode'] = handle.getinfo(pycurl.HTTP_CONNECTCODE)
        m['httpauth-avail'] = handle.getinfo(pycurl.HTTPAUTH_AVAIL)
        m['proxyauth-avail'] = handle.getinfo(pycurl.PROXYAUTH_AVAIL)
        m['os-errno'] = handle.getinfo(pycurl.OS_ERRNO)
        m['num-connects'] = handle.getinfo(pycurl.NUM_CONNECTS)
        m['ssl-engines'] = handle.getinfo(pycurl.SSL_ENGINES)
        m['cookielist'] = handle.getinfo(pycurl.INFO_COOKIELIST)
        m['lastsocket'] = handle.getinfo(pycurl.LASTSOCKET)
        m['ftp-entry-path'] = handle.getinfo(pycurl.FTP_ENTRY_PATH)
        return m

    def _init(self, c, param):

        #不验证证书和host
        c.setopt(pycurl.SSL_VERIFYPEER, False)
        c.setopt(pycurl.SSL_VERIFYHOST, False)

        if param.get('connect_time_out'):
            c.setopt(pycurl.CONNECTTIMEOUT, param['connect_time_out'])
        else:
            c.setopt(pycurl.CONNECTTIMEOUT, 10)

        if param.get('time_out'):
            c.setopt(pycurl.TIMEOUT, param['time_out'])
        else:
            c.setopt(pycurl.TIMEOUT, 20)

        c.setopt(pycurl.FOLLOWLOCATION, 1)
        c.setopt(pycurl.MAXREDIRS, 5)
        c.setopt(pycurl.AUTOREFERER, 1)
        c.setopt(pycurl.NOSIGNAL, 1)
        c.setopt(pycurl.DNS_CACHE_TIMEOUT, 60 * 60)

        head_list = []
        if param.get('user_agent'):
            user_agent = 'User-Agent: ' + param['user_agent']
            head_list.append(user_agent)
        else:
            head_list.append('User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:45.0) Gecko/20100101 Firefox/45.0')

        if param.get('accept'):
            accept = 'Accept: ' + param['accept']
            head_list.append(accept)
        else:
            head_list.append('Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8')

        if param.get('accept_language'):
            accept_language = 'Accept-Language: ' + param['accept_language']
            head_list.append(accept_language)
        else:
            head_list.append('Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3')

        if param.get('accept_encoding'):
            accept_encoding = 'Accept-Encoding: ' + param['accept_encoding']
            head_list.append(accept_encoding)

        head_list.append('Connection: keep-alive')

        if head_list:
            c.setopt(pycurl.HTTPHEADER, head_list)

        self.head_list = head_list

        if param.get('proxy_ip'):
            c.setopt(pycurl.PROXY, param['proxy_ip'])

        if param.get('cookie_file_path'):
            c.setopt(pycurl.COOKIEFILE, param['cookie_file_path'])
            c.setopt(pycurl.COOKIEJAR, param['cookie_file_path'])

        if param.get('cookie'):
            c.setopt(pycurl.COOKIE, param['cookie'])

        return c

    def header(self):
        _header = self.header_buf.getvalue()
        return self.parse_header(_header)

    def parse_header(self, header):
        header_list = header.split("\r\n")
        header_list = header_list[0:-2]
        if '' in header_list:
            blank_index = len(header_list) - 1 - (list(reversed(header_list)).index(''))
            header_list = header_list[blank_index + 1:]

        header_d = {}
        header_d['http_code'] = header_list[0].split(' ')[1]
        for l in header_list[1:]:
            _l = l.split(":")
            name = _l[0].strip()
            value = ''.join(_l[1:]).strip()
            header_d[name.lower()] = value.lower()

        return header_d

    def flush_cookie(self):
        self.get_c.setopt(pycurl.COOKIELIST, 'FLUSH')


class Search(object):
    def __init__(self):
        self._mc = MyCurl()
        self.mc = MyCurl(accept_encoding='gzip, deflate')

    def reset_proxy_ip(self):
        url = 'http://206.99.94.97:8080/wise_proxy/service/main.fcgi?action=get&site=google.ie&type=http&num=1&partition=buy&location='
        _count = 0

        while True:
            try:
                result = self._mc.get_page(url)[1]
            except Exception as e:
                if _count > 3:
                    print 'reset_proxy_ip ' + str(e)
                    logger.error('reset_proxy_ip ' + str(e))
                    result = ''
                    break
                else:
                    _count += 1
                    time.sleep(1)
            else:
                break

        print result
        _proxy_ip = result
        if _proxy_ip:
            _proxy_ip='192.168.40.3:31280'
            #self.mc = MyCurl(proxy_ip='192.168.200.253:3128', accept_encoding='gzip, deflate')
            self.mc = MyCurl(proxy_ip=_proxy_ip, accept_encoding='gzip, deflate')
            self.proxy = _proxy_ip
            #self.mc = MyCurl(proxy_ip='127.0.0.1:8888', accept_encoding='gzip, deflate')
        else:
            self.mc = MyCurl(accept_encoding='gzip, deflate')
            self.proxy = 'NULL'
            logger.error('No available proxy')

    def search(self, url):

        mc = MyCurl(accept_encoding='gzip, deflate', time_out=config.vc_wait_time)
        _count = 0
        while True:
            try:
                self.reset_proxy_ip()
                m, v = self.mc.get_page(url)
            except Exception as e:
                if _count > 3:
                    logger.debug('获取页面错误 超过 3次 返回null')
                    return []
                else:
                    logger.exception(e)
                    _count += 1
                    logger.error('proxy not working..' + self.proxy + str(e))
                    continue

            if m['http-code'] == 403:

                if _count > 3:
                    logger.debug('获取页面错误 超过 3次 返回null')
                    return []
                _count += 1
                logger.error('proxy not working..' + self.proxy + ' : 403')
                continue

            try:
                if m['http-code'] == 503:
                    logger.debug('http_code: 503 需要验证码')

                    count = 0
                    while True:
                        page = gzdecode(v).decode('utf-8', 'ignore')
                        d = pq(page)
                        href = 'https://ipv4.google.com' + d('img').attr('src')
                        logger.debug('img url ' + href)

                        _uuid = uuid.uuid1().get_hex()
                        localtime = time.localtime()
                        tmp_name = str(localtime[0]) + str(localtime[1]) + str(localtime[2]) + _uuid[0:8] + _uuid[16:20]
                        file_path = tmp_name

                        logger.debug('file_path ' + file_path)
                        self.mc.down_file(href, file_path)

                        upload_file_list = []
                        upload_file_list.append(
                            (
                                'image',
                                'imagecode',
                                file_path,
                            )
                        )


                        m, v = mc.upload_file('http://192.168.60.240:8780/verificationCodeFromInputStream', upload_file_list)
                        # os.remove(file_path)
                        result = json.loads(v)

                        logger.debug(result)

                        verificationCode = result['verificationCode']
                        logger.debug('识别出的验证码: %s' % verificationCode)

                        _d = {}
                        for l in d('input').items():
                            if l.attr('type') == 'hidden':
                                k, v = l.attr('name'), l.attr('value')
                                logger.debug(v)
                                _d[k] = v

                        url = 'https://ipv4.google.com/sorry/CaptchaRedirect?q=' + _d['q'] + '&continue=' + urllib.quote_plus(_d['continue']) + '&id=' + _d['id'] \
                            + '&captcha=' + result['verificationCode'] + '&submit=' + urllib.quote('提交')
                        logger.debug('submit url ' + href)

                        m, v = self.mc.get_page(url)
                        logger.debug(m)
                        logger.debug(v)

                        if m['http-code'] == 503:
                            logger.debug('验证码识别错误')
                            if count > 3:
                                logger.debug('验证码识别错误 超过 3次 返回null')
                                return []
                            else:
                                count += 1
                                continue
                        else:
                            break

                break
            except Exception as e:
                logger.error(str(e) + '  具体栈回溯信息查看crit.log  ')
                logger.exception(e)
                # if file_path:
                    # os.remove(file_path)
                return []

        page = gzdecode(v).decode('utf-8', 'ignore')
        logger.debug(page)
        d = pq(page)
        _l = []
        for l in d('.r>a').items():
            _l.append(l.attr('href'))

        return _l


class ServerThread(threading.Thread):
    def __init__(self):
        super(type(self), self).__init__()
        self.setDaemon(True)
        self.start()

    class Handler(StreamRequestHandler):

        req_num = 0
        lock = threading.Lock()

        def get_page_link_list(self, url):
            #mc = MyCurl(proxy_ip='192.168.200.253:3128', accept_encoding='gzip, deflate')
            #mc = MyCurl(proxy_ip='127.0.0.1:8888', accept_encoding='gzip, deflate')
            mc = MyCurl(accept_encoding='gzip, deflate')
            pic_regex = re.compile(r'.+\.(jpg|jpeg|gif|png|bmp|xml|json|swf|zip)$')
            _a_list = []

            scheme, netloc = urlparse.urlparse(url)[:2]

            if scheme not in ('http', 'https'):
                return []

            try:
                count = 0
                while True:
                    h, page = mc.get_page(url)
                    #对于q=xxx site:domain.xxx之类的搜索 搜索结果如果有100条 频繁的访问domain.xxx 会导致503 尤其是这个网站用了cf之类的
                    if h['http-code'] == 503:
                        if count > 2:
                            return []
                        else:
                            time.sleep(3)
                            count += 1
                            continue
                    else:
                        break

                page = gzdecode(page)
                content_type = h['content-type']
                if 'text/html' not in content_type:
                    print 'content_type: %s' % content_type
                    return []

                r = re.search(r'charset=(.+)', content_type)
                if not r:
                    charset = 'utf-8'
                else:
                    charset = r.group(1)

                page = page.decode(charset, 'ignore')

                d = pq(page)
                a_list = d('a, iframe')
                for l in a_list.items():
                    if l[0].tag == 'iframe':
                        href = l.attr('src')
                        if not href:
                            continue
                    else:
                        href = l.attr('href')
                        if not href:
                            continue

                    if 'javascript' in href:
                        continue

                    if pic_regex.search(href.lower()):
                        continue

                    href = 'http:' + href if href[:2] == '//' else href

                    _scheme, _netloc = urlparse.urlparse(href)[:2]
                    if _scheme and (_scheme not in ('http', 'https')):
                        continue

                    if not _scheme and not _netloc:
                        href = scheme + '://' + netloc + ('' if href[0] == '/' else '/') + href

                    _a_list.append(href)
            except Exception as e:
                logger.error('get_page_link_list %s %s' % (url, str(e)))
                logger.exception(e)
            else:
                pass
                # try:
                #     if not _a_list:
                #         _uuid = uuid.uuid1().get_hex()
                #         localtime = time.localtime()
                #         tmp_name = str(localtime[0]) + str(localtime[1]) + str(localtime[2]) + _uuid[0:8] + _uuid[16:20]
                #         with open(tmp_name + '.html', 'w+') as f:
                #             f.write(url + '\n')
                #             f.write(page)

                #         with open(tmp_name + '2.html', 'w+') as f2:
                #             f2.write(url + '\n')
                #             f2.write(str(d))
                # except Exception as e:
                #     logger.error('if not _a_list %s %s' % (url, str(e)))
                #     logger.exception(e)

            return [(l, url) for l in list(set(_a_list))]

        # def func(self, link_url):
        #     pool = self.server.pool

        #     link_url = link_url.strip()
        #     if not link_url:
        #         logger.debug('google search url null')
        #         return ''

        #     list_url = self.get_page_link_list(link_url)
        #     logger.debug('req_num: %d, link_url: %s, has %d url[begin]' % (self.req_num, link_url, len(list_url)))
        #     r = reduce(lambda _list, elem: _list.extend(elem) or _list, pool.map(self.get_page_link_list, [l[0] for l in list_url]), list())
        #     for l in list_url:
        #         r.append((l[0], link_url))
        #     logger.debug('req_num: %d, link_url: %s, has %d url[end]' % (self.req_num, link_url, len(list_url)))
        #     return r

        def get_num(self):
            self.lock.acquire()
            self.__class__.req_num += 1
            req_num = self.__class__.req_num
            self.lock.release()
            return req_num

        def handle(self):
            pool = self.server.pool

            data = self.request.recv(1024)
            if not data:
                logger.debug('self.request.recv null')
                return

            try:
                data = json.loads(data)
                search_url = data['url']
            except:
                logger.debug('data error')
                return

            try:
                self.req_num = self.get_num()
                logger.debug('[begin]req_num: %d, search url: %s' % (self.req_num, search_url))
            except Exception as e:
                logger.debug('search url: %s, error %s' % (search_url, str(e)))
                logger.exception(e)

            try:
                #extraInfo = data['extraInfo']
                if not search_url:
                    logger.debug('search_url null')
                    logger.debug('[end]req_num: %d, search url: %s' % (self.req_num, search_url))
                    self.request.send('')
                else:
                    link_url_list = Search().search(search_url)

                    logger.debug('req_num: %d, google搜索返回的记录数 %d' % (self.req_num, len(link_url_list)))

                    link_url_list = ['http:' + l if l[:2] == '//' else l for l in link_url_list]
                    #link_url_list = [(l, req_num) for l in link_url_list]
                    _results = []
                    for l in link_url_list:
                        list_url = self.get_page_link_list(l)
                        if config.crawl_level == 2:
                            result = reduce(lambda _list, elem: _list.extend(elem) or _list, pool.map(self.get_page_link_list, [l[0] for l in list_url]), list())
                            for j in list_url:
                                result.append((j[0], l))
                            _results.extend(result)
                        else:
                            for j in list_url:
                                _results.append((j[0], l))

                    d = {}
                    d['request'] = data
                    d['response'] = _results
                    #with open('out.json', 'w+') as f:
                    for l in link_url_list:
                        d['response'].append((l, search_url))

                    logger.debug('[end]req_num: %d, search url: %s, 搜索到的记录数 %d' % (self.req_num, search_url, len(d['response'])))
                    out_str = json.dumps(d)
                    self.request.send(out_str)
            except Exception as e:
                logger.debug('[end]req_num: %d, search url: %s, error %s' % (self.req_num, search_url, str(e)))
                logger.exception(e)

        # def send_response(self, res):
        #     logger.debug('send_response: %d' % len(res))
        #     try:
        #         connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
        #         channel = connection.channel()
        #         channel.queue_declare(queue='google_search')
        #         channel.basic_publish(exchange='', routing_key='google_search', body=res)
        #         connection.close()
        #     except Exception as e:
        #         logger.error(str(e))
        #         logger.exception(e)

    class DebugThread(threading.Thread):
        def __init__(self, pool):
            super(type(self), self).__init__()
            self.pool = pool
            self.setDaemon(True)
            self.start()

        def run(self):
            logger.info('debug thread start!!!')
            while True:
                logger.info('pool state %d' % self.pool._state)
                for l in self.pool._pool:
                    print l.is_alive(), l.name
                print len(self.pool._pool)
                time.sleep(5)

    def run(self):
        logger.info('server thread start!!!')
        try:
            self.server = ThreadingTCPServer(('127.0.0.1', 50005), self.Handler)
            self.server.pool = ThreadPool(config.thread_num)
            #self.DebugThread(self.server.pool)
            self.server.serve_forever()
        except Exception as e:
            logger.error(str(e) + '  具体栈回溯信息查看crit.log  ')
            logger.exception(e)

    def shut_down(self):
        self.server.shutdown()


class Task(object):
    def __init__(self):
        pass

    def signal_handler(self, signum, frame):
        if signum == signal.SIGTERM:
            #self.term_flag = True
            self.st.shut_down()
        else:
            pass

    def run(self):
        logger.info('task start!!!')
        self.st = ServerThread()
        self.st.join()


if __name__ == '__main__':
    Task().run()
    # r = get_page_link_list('http://www.movie4k.to/Dorfman-in-Love-watch-movie-3258225.html')
    # for l in r:
    #     print l[0]
