#/usr/bin/env python
#coding:utf-8
import functools
def decorator(func):
  @functools.wraps(func)
  def wrapper(*args,**kw):
    print('begin %s():'%func.__name__)
    out=func(*args,**kw)
    print('end')
    return out
  return wrapper
@decorator
def now():
  print("2015-6-12")

if __name__=="__main__":
  now()
  
