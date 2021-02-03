#!/usr/bin/env python3

import kivy
import os
kivy.require('2.0.0')

from os import system

from kivy.app import App
from kivy.uix.widget import Widget
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.uix.togglebutton import ToggleButton
from kivy.uix.dropdown import DropDown
from kivy.uix.gridlayout import GridLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.pagelayout import PageLayout
from kivy.uix.screenmanager import ScreenManager, Screen

from kivy.lang import Builder

from kivy.config import Config

from kivy.core.window import Window

# Setting app size (Pi Touchscreen uses 800x480)
Config.set('graphics', 'width', '1920')
Config.set('graphics', 'height', '1080')
Builder.load_file('PillTankRoot.kv')
Window.fullscreen = True

# class DetectButton(Button):
#     def build(self):
#         HomeScreen.build.layout.add_widget(DetectButton)
#     #     self.bind(on_press = self.callback)
#     def callback(self):
#         os.system('sudo ../vectorizedUnlooped/vectorizedSobel')
#         # os.system('../Camera++/Camera')

# class HistoryButton(Button):
#     def build(self):
#         HomeScreen.build.layout.add_widget(HistoryButton)
#     def on_release(self):
#         App.get_running_app().root.current = 'History'
# class ProfileButton(Button):
#     def build(self):
#         HomeScreen.build.layout.add_widget(ProfileButton)
#     def callback(self):
#         pass
        

# class SettingsButton(Button):
#     def build(self):
#         HomeScreen.build.layout.add_widget(SettingsButton)

class PillTankRoot(BoxLayout):
    #Root Class
    def __init__(self, **kwargs):
        super(PillTankRoot, self).__init__(**kwargs)
        self.screen_list = []
        sm = ScreenManager()
        sm.add_widget(History(name = 'History'))
        sm.add_widget(Profile(name = 'Profile'))
        sm.add_widget(SettingsScreen(name = 'SettingsScreen'))
    
    def changeScreen(self, next_screen):
        screens = "Profile History SettingsScreen".split()
        if next_screen == "History":
            self.ids.screen_manager.current = "History"
        elif next_screen == "HomeScreen":
            self.ids.screen_manager.current = "HomeScreen"
        elif next_screen == "SettingsScreen":
            self.ids.screen_manager.current = "SettingsScreen"
        elif next_screen == "Profile":
            self.ids.screen_manager.current = "Profile"
        pass

    def launch(self):
        os.system('sudo ../vectorizedUnlooped/vectorizedSobel')


# HomeScreen
class HomeScreen(App):
    def __init__(self,**kwargs):
        super(HomeScreen, self).__init__(**kwargs)

    def build(self):
        return PillTankRoot()
    

# Other Screens
class History(Screen):
    def __init__(self, **kwargs):
        super(History, self).__init__(**kwargs)
    
    def build(self):
        pass

class Profile(Screen):
    def __init(self, **kwargs):
        super(Profile,self).__init__(**kwargs)
    
    def build(self):
        pass

class SettingsScreen(Screen):
    def __init(self, **kwargs):
        super(SettingsScreen,self).__init__(**kwargs)
    
    def build(self):
        pass

if __name__ == '__main__':
    HomeScreen().run()