import kivy
from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.gridlayout import GridLayout
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
myList = []
class MyGrid(GridLayout):
    def __init__(self, **kwargs):
        super(MyGrid, self).__init__(**kwargs)
        self.cols = 4
        self.add_widget(Label(text = "Address: "))
        self.address = TextInput(multiline = False)
        self.add_widget(self.address)

        

        self.submit = Button(text = "Submit", font_size = 40)
        self.submit.bind(on_press = self.pressed)
        self.add_widget(self.submit)
    def pressed(self, instance):
        address = self.address.text
        myList.append(address)
        from pytube import YouTube
        flag = 1
        url = myList[0]
        cnt = 0
        while (flag and url) :
            try:
                YouTube(url).streams.first().download()
                flag = 0
                print("成功")
            except:
                cnt += 1
                print("失敗 ",cnt)
                flag = 1
    
class HelloApp(App):
    def build(self):
        return MyGrid()
    

if __name__ == '__main__':
    HelloApp().run()
