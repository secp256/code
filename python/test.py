#!/usr/bin/python

def format_digit(s):
  splitter = ""
  if '-' in s:
    splitter = "-"
  elif ':' in s:
    splitter = ":"
  else:
    return s

  (minute, second) = s.split(splitter)
  return minute + "." + second

def get_coach_data(filename):
  try:
    with open(filename) as f:
        data = f.readline()
    return data.strip().split(',')
  except IOError as ioerr:
    print("file rooro: " + str(ioerr))
    return None

def main():
  filenames = ["hfpy_ch5_data/james.txt", "hfpy_ch5_data/julie.txt", "hfpy_ch5_data/mikey.txt", "hfpy_ch5_data/sarah.txt"]
  for filename in filenames:
      data = get_coach_data(filename)
      print "origin: " , data 
      print "sorted: " , sorted([format_digit(item) for item in data], reverse = True)
      print

if __name__ == '__main__':
  main()
