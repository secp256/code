#!/usr/bin/env python
# -*- coding: utf-8 -*-
import time
import textwrap  
import tornado.httpserver   
import tornado.ioloop   
import tornado.options   
import tornado.web
import tornado.gen
from tornado.options import define, options   

define("port", default=8000, help="run on the given port", type=int)   

class MainHandler(tornado.web.RequestHandler):   
  def get(self):   
    self.write('<html><body><form action="/" method="post">'
        '<input type="text" name="message">'
        '<input type="submit" value="Submit">'
        '</form></body></html>')

  def post(self):
    self.set_header("Content-Type", "text/plain")
    print self.get_argument("message")
    self.write("You wrote " + self.get_argument("message"))

class WrapHandler(tornado.web.RequestHandler):   
  def get(self, id):   
    #self.set_header("Content-Type", "text/plain")
    self.write("your request id is: " + id)

class BadHandler(tornado.web.RequestHandler):
  def get(self):
    for i in range(5):
      print(i)
      #time.sleep(1)
      yield gen.sleep(1)

if __name__ == "__main__":   
  tornado.options.parse_command_line()   
  app = tornado.web.Application(   
      handlers=[   
        (r"/", MainHandler),   
        (r"/wrap/([0-9]+)", WrapHandler),
        (r"/bad", BadHandler)  
        ]
      )   
  http_server = tornado.httpserver.HTTPServer(app)   
  http_server.listen(options.port)   
  tornado.ioloop.IOLoop.instance().start()
