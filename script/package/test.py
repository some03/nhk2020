#!/usr/bin/python2
import yaml
import sys
from pathlib2 import Path

sys.path.append(str(Path('__file__').resolve().parent.parent))
data_raw='../../route/'

with open(data_raw+'waypoints1.yaml') as File:
    y=yaml.safe_load(File.read())
    print(y['waypoints'][0]['position']['x'])


