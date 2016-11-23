#!/usr/bin/env python3
#coding: utf-8

import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.header import Header
import threading

def send_mail(thread_id):
  sender = 'sender@sina.com'
  receiver = 'recv@xxx.cn'

  subject = 'Hi, benbenben'
  mail_body = 'Hi, test!  %s' % thread_id

  username = 'sender@sina.com'
  password = 'xxx_sasa'

  message = MIMEMultipart()
  # message['From'] = sender
  # message['To'] = receiver
  message['From'] = ''
  message['To'] = ''
  message['Subject'] = Header(subject, 'utf-8')
  message.attach(MIMEText(mail_body, 'plain', 'utf-8'))

  smtp = smtplib.SMTP()
  smtp.connect('smtp.sina.com')
  smtp.login(username, password)
  smtp.sendmail(sender, receiver, message.as_string())
  smtp.quit()

def main():
  process_num = 2

  threads = []
  for i in range(process_num):
    thread = threading.Thread(target = send_mail, args = (i, ))
    threads.append(thread)

  for thread in threads:
    thread.start()
  for thread in threads:
    thread.join()

if __name__ == "__main__":
  main()
