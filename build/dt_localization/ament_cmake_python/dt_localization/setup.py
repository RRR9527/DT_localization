from setuptools import find_packages
from setuptools import setup

setup(
    name='dt_localization',
    version='0.0.0',
    packages=find_packages(
        include=('dt_localization', 'dt_localization.*')),
)
