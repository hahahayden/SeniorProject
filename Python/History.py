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



class History(App):

    def build(self):
        layout = PageLayout
        self.most_recent = Builder.load_file('MostRecent.kv')
    pass