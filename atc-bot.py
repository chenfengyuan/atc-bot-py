#!/usr/bin/env python3
# coding=utf-8
__author__ = 'chenfengyuan'
import cffi
import requests
import time


class Search():

    def __init__(self):
        ffi = cffi.FFI()
        ffi.cdef("void * make_cache();")
        ffi.cdef("void free_cache(void *);")
        ffi.cdef("char const * search(void *, char *);")
        self.ffi = ffi
        self.atc_bot = ffi.dlopen('./libatc-bot-py.dylib')
        self.cache = self.atc_bot.make_cache()

    def get_action(self, data):
        p = self.atc_bot.search(self.cache, data)
        return self.ffi.string(self.ffi.cast('char *', p)).decode('ascii')

    def __del__(self):
        self.atc_bot.free_cache(self.cache)


def main():
    search = Search()
    pre_data = ''
    while True:
        resp = requests.get('http://10.211.55.17:14200')
        data = resp.text.encode('utf-8')
        if data == pre_data:
            time.sleep(0.01)
            continue
        pre_data = data
        start = time.time()
        action = search.get_action(resp.text.encode('utf-8'))
        print('%s %s' % (time.time() - start, action))
        requests.post('http://10.211.55.17:14200', data=action)


if __name__ == '__main__':
    main()
