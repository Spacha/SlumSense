import sys
import platform
import asyncio
import logging

from bleak import BleakClient

logger = logging.getLogger(__name__)

VERBOSE = False
ADDRESS = (
    "ff:d0:eb:ad:62:65"
    #"24:71:89:cc:09:05"
    #if platform.system() != "Darwin"
    #else "B9EA5233-37EF-4DD6-87A8-2A875E821C46"
)

async def readData(address, characteristics):
    char_queue = characteristics.copy()
    data = []
    async with BleakClient(address) as client:
        logger.info(f"Connected: {client.is_connected}")

        for service in client.services:
            logger.info(f"[Service] {service}")
            for char in service.characteristics:
                if "read" in char.properties:
                    try:
                        value = bytes(await client.read_gatt_char(char.uuid))
                        
                        # if correct UUID is found, get the value and leave
                        if char.uuid in char_queue:
                            data.append(int.from_bytes(value, "big")) # check endiannes

                            # remove the UUId from the list to speed up next loops
                            char_queue.remove(char.uuid)
                            if len(char_queue) == 0:
                                return data # all found


                        logger.info(
                            f"\t[Characteristic] {char} ({','.join(char.properties)}), Value: {value}"
                        )
                    except Exception as e:
                        logger.error(
                            f"\t[Characteristic] {char} ({','.join(char.properties)}), Value: {e}"
                        )

                else:
                    value = None
                    logger.info(
                        f"\t[Characteristic] {char} ({','.join(char.properties)}), Value: {value}"
                    )

    return data

async def main(address):
    labels = ["temperature"]
    char_uuids = ["00002aa6-0000-1000-8000-00805f9b34fb"]
    values = (await readData(address, char_uuids))

    for i,e in enumerate(labels):
        print("{}: {} ({})".format(labels[i], values[i], char_uuids[i]))


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    logger.propagate = VERBOSE
    asyncio.run(main(sys.argv[1] if len(sys.argv) == 2 else ADDRESS))
