import pyttsx3
import speech_recognition as sr
import time
import json
import threading
import pyaudio
import wave
import pyjokes
import os
from home import home
from wiki import get_wikipedia

wave_file = "audio.wav"

devices = ["bathroom one", "bathroom two", "bathroom three", "bathroom downstairs", "office", "bedroom one", "bedroom two", "bedroom three", "garage", "kitchen", "pantry"]

def record():
    duration = 10
    fs = 44100

    audio = pyaudio.PyAudio()
    stream = audio.open(format=pyaudio.paInt16,
                        channels=1,
                        rate=fs,
                        input=True,
                        frames_per_buffer=1024)

    frames = []
    for i in range(0, int(fs / 1024 * duration)):
        data = stream.read(1024)
        frames.append(data)

    stream.stop_stream()
    stream.close()
    audio.terminate()

    global wave_file
    with wave.open(wave_file, 'wb') as wf:
        wf.setnchannels(1)
        wf.setsampwidth(audio.get_sample_size(pyaudio.paInt16))
        wf.setframerate(fs)
        wf.writeframes(b''.join(frames))



def say(*arg):
    engine = pyttsx3.init()
    voices = engine.getProperty('voices')
    engine.setProperty('voice', voices[1].id)
    engine.setProperty('rate', 150)

    texto = ' '.join(arg)
    engine.say(texto)
    time.sleep(0.5)
    engine.runAndWait()

def process_command(command):
    lines = command.split()
    n = -1
    global devices
    n = n+1
    if "turn" in command:
        if "on" in command:
            for device in devices:
                if device in command and ("lights" in command or "light" in command):
                    if " " in device:
                        device = device.replace(" ", "_")
                    home.turn_on(f"light.{device}")
                    say("ok, turning on the lights")
        elif "off" in command:
            for device in devices:
                if device in command and ("lights" in command or "light" in command):
                    if " " in device:
                        device = device.replace(" ", "_")
                    home.turn_off(f"light.{device}")
                    say("ok, turning off the lights")
    elif "temperature" in command or "hot" in command:
        temperature = home.get_data("sensor.hyt_upstairs")
        temperature = temperature.split(",")
        temperature = temperature[0].replace("Temp:", "")
        say(f"Is {temperature} outside right now")
    elif "humidity" in command:
        humidity = home.get_data("sensor.hyt_upstairs")
        humidity = humidity.split(",")
        humidity = humidity[1].replace("Humidity:", "").replace("%", "")
        say(f"The humidity outside is {humidity}%")
    elif "who is" in command:
        command = command.split("who is")
        say(get_wikipedia(command[1], summary_ratio=0.08))
    elif "what is" in command:
        command = command.split("what is")
        say(get_wikipedia(command[1], summary_ratio=0.08))
    elif "what's the meaning of" in command:
        command = command.split("what's the meaning of")
        say(get_wikipedia(command[1], summary_ratio=0.08))
    elif "joke" in command:
        say(pyjokes.get_joke())
    elif "kill" in command:
        say("oh ok. sad face")
        os._exit(0)

    else:
        say("Unknown command")


listener = sr.Recognizer()

def recognize():
    global wave_file
    record()
    try:
        with sr.AudioFile(wave_file) as source:
            audio = listener.record(source)
        recognized_text = listener.recognize_google(audio)
        command = recognized_text.lower()
        print(command)
        if "jarvis" or "darby's" or "daddy's "in command:
            process_command(command)
        else:
            print("Unknown command")
    except Exception as e:
        print(e)
        print("I dint recognize")
say("hi im Jarvis, nice to meet you")
while True:
    recognize()
