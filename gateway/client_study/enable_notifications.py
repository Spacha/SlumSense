# -*- coding: utf-8 -*-
"""
Notifications
-------------
Example showing how to add notifications to a characteristic and handle the responses.
Updated on 2019-07-03 by hbldh <henrik.blidh@gmail.com>
"""

import os
import sys
import asyncio
import platform
import requests
from hashlib import md5

from bleak import BleakClient

# SlumCloud API
API_URL     = "https://slum.spacha.dev/api/v1"
GATEWAY_KEY = "aQXcflqN"

# you can change these to match your device or override them from the command line
CHARACTERISTIC_UUID = "f3641401-00b0-4240-ba50-05ca45bf8abc"
ADDRESS = (
    "ff:d0:eb:ad:62:65"
    #if platform.system() != "Darwin"
    #else "B9EA5233-37EF-4DD6-87A8-2A875E821C46"
)

def get_hashed_api_key():
    ''' Get APi ke '''
    fail = False
    api_key = ""
    try:
        api_key = os.environ["API_KEY"]
    except Exception:
        fail = True

    if fail or len(api_key) > 3:
        return md5(api_key.encode()).hexdigest()
    else:
        raise Exception("Missing API key! Set it as an environment variable 'API_KEY'.")

def send_data(temp, pres, humi):
    response = requests.post("{API_URL}/measurements", data = {
        "API_KEY":      get_hashed_api_key(),
        "gateway_key":  GATEWAY_KEY,
        "temperature":  temp,
        "pressure":     temp,
        "humidity":     humi
    })

    print(response)
    print("Status: ", + str(response.status_code))
    print("Message: " + response.text)


def notification_handler(sender, data):
    """Simple notification handler which prints the data received."""
    # print("{0}: {1}".format(sender, data))
    data_int = int.from_bytes(data, byteorder='little')
    #temp= data_int
    temp = (data_int & 0x00000000ffff) >> 0
    pres = (data_int & 0x0000ffff0000) >> 16
    humi = (data_int & 0xffff00000000) >> 32

    print("0x{:012x}".format(data_int))

    print("{}\t => {} °C".format( temp, round((temp/100.0 - 70), 2) ))
    print("{}\t => {} hPa".format( pres, round(pres/10.0, 2) ))
    print("{}\t => {} %".format( humi, round(humi/10.0, 2) ))

    send_data(temp, pres, humi)



async def main(address, char_uuid):
    async with BleakClient(address) as client:
        print(f"Connected: {client.is_connected}")

        # loop = asyncio.get_event_loop()

        await client.start_notify(char_uuid, notification_handler)

        while(True):
            await asyncio.sleep(60.0)

        # await asyncio.sleep(9999.0)
        # await client.stop_notify(char_uuid)


if __name__ == "__main__":
    asyncio.run(
        main(
            sys.argv[1] if len(sys.argv) > 1 else ADDRESS,
            sys.argv[2] if len(sys.argv) > 2 else CHARACTERISTIC_UUID,
        )
    )
