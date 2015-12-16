#!/usr/bin/env python
# -*- coding: utf-8 -*-

# http://bottlepy.org/docs/dev/tutorial.html

from bottle import route, run, template
from bottle import get, post, request
from bottle import static_file
from bottle import error

@route('/')
@route('/hello/<name>')
def index(name = 'guest'):
  return template('<b>Hello {{name}}, how are you ?</b>!', name=name)
# http://localhost:8080/static/google.png

@route('/static/<filename>')
def server_static(filename):
  return static_file(filename, root='/home/zhuliting/Desktop/')

@error(404)
def error404(error):
  return 'nothing here, sorry'

@get('/login') # or @route('/login')
def login():
  return '''
  <form action="/login" method="post">
  Username: <input name="username" type="text" /><br>
  Password: <input name="password" type="password" /><br>
  <input value="Login" type="submit" />
  </form>
'''

@post('/login')
def do_login():
  username = request.forms.get('username')
  password = request.forms.get('password')
  if check_login(username, password):
    return "<p>Your login information was correct.</p>"
  else:
    return "<p>Login failed.</p>"

@route('/upload', method='POST')
def do_upload():
  category = request.forms.get('category')
  upload = request.files.get('upload')
  name, ext = os.path.splitext(upload.filename)

  if ext not in('.png', '.jpg', '.jpeg'):
    return 'file extension not allowed.'

  save_path = get_save_path_for_category(category)
  upload.save(save_path)
  return 'OK'

@route('/ip')
def show_ip():
  ip = request.environ.get('REMOTE_ADDR')
  return template('Your ip is: {{ip}}\n', ip = ip)

def check_login(username, password):
  return True

def main():
  run(host='192.168.5.94', port=8080)

if __name__ == '__main__':
  main()
