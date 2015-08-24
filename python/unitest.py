#!/usr/bin/python

import unittest
from selenium import webdriver


class TestTwo(unittest.TestCase):
  def setUp(self):
    self.driver = webdriver.PhantomJS()

  def test_url(self):
    self.driver.get("https://app.simplegoods.co/i/IQCZADOY") # url associated with button click
    button = self.driver.find_element_by_id("payment-submit").get_attribute("value")
    self.assertEquals(u'Pay - $60.00', button)

  def tearDown(self):
    self.driver.quit()

if __name__ == '__main__':
  unittest.main()
