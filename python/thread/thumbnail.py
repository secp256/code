#!/usr/bin/env python
# -*- coding: utf-8 -*-

from multiprocessing import Pool
from PIL import Image
import os

SIZE = (75, 75)
SAVE_DIRECTORY = 'thumbs'

def get_image_paths(folder):
  return (os.path.join(folder, f)
      for f in os.listdir(folder) if f[-4:] == '.jpg'
          )

def create_thumbnail(filename):
  #print filename
  im = Image.open(filename)
  im.thumbnail(SIZE, Image.ANTIALIAS)
  base, fname = os.path.split(filename)
  save_path = os.path.join(base, SAVE_DIRECTORY, fname)
  im.save(save_path)

if __name__ == '__main__':
  folder = os.path.abspath(
      '/home/zhuliting/src/code/shell/bing/'
          )
  # os.rmdir(os.path.join(folder, SAVE_DIRECTORY))
  os.mkdir(os.path.join(folder, SAVE_DIRECTORY))
  images = get_image_paths(folder)

  # for image in images:
  #   create_thumbnail(image)
  pool = Pool()
  pool.map(create_thumbnail, images)
  pool.close()
  pool.join()
