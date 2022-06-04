import json
import requests
import loadSecrets as secrets
from configparser import ConfigParser




def getTempandHum(lat, lon):
    api_key = secrets.load_secrets("","CEIoT","openweather_key")
    baseurl = "https://api.openweathermap.org"
    query = {'lat':lat, 'lon':lon, 'units':'metric', 'appid':api_key}
    try:
        response = requests.get(baseurl+"/data/2.5/weather", params=query).json()
    except:
        response = {
            "main": {
                "temp":"error",
                "humidity":"error"
            }
        }
    return response

def postTempandHum(id, record):
    api_url = "http://192.168.1.203:8080/measurement2"
    api_headers = {"content-type": "application/x-www-form-urlencoded"}
    #api_url = "https://webhook.site/879bebe6-c35a-4258-9c55-0e354d636fce"
         # print(record)
    # print(record["main"])
    temp = record['main']['temp']
    hum = record['main']['humidity']
    ts = record["dt"] 
    api_body = "id="+ str(id) + "&t=" + str(temp) + "&h="+ str(hum) + "&ts=" + str(ts)
    response = requests.post(api_url, headers=api_headers, data=api_body )
    print(response)


if __name__ == '__main__':
     lon= -95.5415
     lat= 30.1462
     id=77
     response = getTempandHum(lat, lon)
    #  response =  {'coord':{'lon':-95.5415,'lat':30.1462},'weather':[{'id':801,'main':'Clouds','description':'few clouds','icon':'02d'}],'base':'stations','main':{'temp':31.63,'feels_like':35.95,'temp_min':30.15,'temp_max':33.31,'pressure':1009,'humidity':59},'visibility':10000,'wind':{'speed':4.63,'deg':170,'gust':10.29},'clouds':{'all':20},'dt':1653845246,'sys':{'type':2,'id':2006722,'country':'US','sunrise':1653823331,'sunset':1653873440},'timezone':-18000,'id':4736476,'name':'The Woodlands','cod':200}
     postTempandHum(id, response)
    #  print (response)
