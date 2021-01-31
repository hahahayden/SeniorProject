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

from kivy.lang import Builder

from kivy.config import Config

from kivy.core.window import Window

class MostRecent(DropDown):
    def build(self):
        HistoryScreen.build.layout.add_widget(MostRecent)
    def callback(self):
        Homescreen.HomeScreen().run()
        pass
    pass

class BackButton(Button):
    def build(self):
        HistoryScreen.build.layout.add_widget(BackButton)
    


class HistoryScreen(App):

    def __init__(self,**kwargs):
        super(HistoryScreen,self).__init__(**kwargs)

    def build(self):
        layout = PageLayout()

        self.MostRecent = Builder.load_file('MostRecent.kv')
        self.BackButton = Builder.load_file('ReturnButton.kv')

        layout.add_widget(self.MostRecent)
        layout.add_widget(self.BackButton)

        return layout
    pass