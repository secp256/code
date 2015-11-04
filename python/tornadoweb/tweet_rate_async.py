#!/usr/bin/env python
# -*- coding: utf-8 -*-

import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web
import tornado.httpclient

import urllib
import json
import datetime
import time

from tornado.options import define, options
define("port", default=8000, help="run on the given port", type=int)

class IndexHandler(tornado.web.RequestHandler):
  @tornado.web.asynchronous
  def get(self):
    self.query = self.get_argument('q')
    client = tornado.httpclient.AsyncHTTPClient()
    url = "http://v.youku.com/player/getPlayList/VideoIDS/XNTkyMjExNDM2/timezone/+08/version/5/source/out"
    client.fetch(url, callback = self.on_response)

  def on_response(self, response):
    body = json.loads(response.body)
    result_count = body['data'][0]['seed']
    self.write("""
    <div style="text-align: center">
    <div style="font-size: 72px">%s</div>
    <div style="font-size: 144px">%.02f</div>
    </div>""" % (self.query, result_count))
    self.finish()

if __name__ == "__main__":
   tornado.options.parse_command_line()
   app = tornado.web.Application(handlers=[(r"/", IndexHandler)])
   http_server = tornado.httpserver.HTTPServer(app)
   http_server.listen(options.port)
   tornado.ioloop.IOLoop.instance().start()
