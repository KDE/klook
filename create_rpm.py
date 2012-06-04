#!/usr/bin/env python

import os
import tarfile
import shutil
import subprocess

version = '1.0'
name = 'klook'

files = ['src', 'CMakeLists.txt', 'locale', 'icons']

src_name = name + '-' +  version + '.tar.gz'
folder = name + '-' + version + '/'

rpmdir = os.path.expanduser('~') + '/rpmbuild/'

with tarfile.open(rpmdir + 'SOURCES/' + src_name, 'w:gz') as tar:
    for file in files:
        tar.add(file)
    tar.close()

shutil.copyfile(name + '.spec', rpmdir + 'SPECS/' + name + '.spec') 

os.chdir(rpmdir + 'SPECS/')

subprocess.call('rpm -bb ' + name + '.spec', shell=True)
subprocess.call('rpm -bs ' + name + '.spec', shell=True)

