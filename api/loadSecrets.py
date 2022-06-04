from configparser import ConfigParser
from pandas import isnull

def load_secrets(filename, section, secret):
    """ Loads secret value stored in the filename provided (default to secrets.ini) """
    if filename == "":
       #filename ='~/esp/ceiot_base/config/secrets.ini'
       filename ='/home/martin/esp/ceiot_base/config/secrets.ini'
       #filename ='./secrets.ini'
    # create a parser
    parser = ConfigParser()
    # read config file
    parser.read(filename)
    key = {}
    if parser.has_section(section):
        params = parser.items(section)
        for param in params:
            key[param[0]] = param[1]
    else:
        raise Exception('Section {0} not found in the {1} file'.format(section, filename))

    return key[secret]
