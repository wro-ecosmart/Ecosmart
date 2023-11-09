from requests import post, get
import json


token = "HOME ASSISTANT TOKEN"

class home():
    def turn_on(id):
        global token
        url = "http://192.168.1.123:8123/api/services/light/turn_on"
        headers = {"Authorization": f"Bearer {token}"}
        data = {"entity_id": id}
        response = post(url, headers=headers, json=data)
        print(response.text)

    def turn_off(id):
        global token
        url = "http://192.168.1.123:8123/api/services/light/turn_off"
        headers = {"Authorization": f"Bearer {token}"}
        data = {"entity_id": id}
        response = post(url, headers=headers, json=data)
        print(response.text)

    def get_data(id):
        global token
        url = f"http://192.168.1.123:8123/api/states/{id}"
        headers = {"Authorization": f"Bearer {token}", "content-type": "application/json"}
        response = get(url, headers=headers)
        print(response.text)
        data = json.loads(response.text)
        print(data["state"])
        return data["state"]

if __name__ == "__main__":
    home.turn_off("light.office")
