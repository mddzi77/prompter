import serial
import time
import datetime as dt


class Text:

    def __init__(self):

        # prompted
        with open('alphabet.txt', encoding='UTF-8') as f:
            self.alphabet = f.read().split('\n')

        self.characters = '1234567890_QWERTYUIOPASDFGHJKLZXCVBNMĄĆĘŁŃÓŚŻŹ'
        self.text = self.text_input()  # text to prompt
        self.image = self.to_chars(self.text)  # image from chars of text to prompt

    def text_input(self):
        """
        Text input loop
        :return: The text given in output
        """
        # input loop
        while True:
            try:
                text = input('Write your prompt -> ').upper().replace(' ', '_')

                # check if all characters are possible to display
                for t in text:
                    if t not in self.characters:
                        raise ValueError(t)
            except ValueError as err:
                print(f'Character \'{err.args[0]}\' not supported, possible characters:')
                print([c for c in self.characters.replace('_', '~space~')])
            else:
                # break loop when all characters are supported
                break

        return text

    def to_chars(self, text: str, pause=True):
        """
        Convert text to 'ascii' image
        :param text: Text to convert
        :param pause: Add 8 chars width pause at the end
        :return: 'Ascii' image of given text
        """
        image = ['' for i in range(8)]
        for t in text:
            for i in range(8):
                index = self.alphabet.index(t) + 1  # starting index of letter image from chars
                image[i] += (self.alphabet[index: index + 8][i])

        if pause:
            # add pause after text
            for i in range(8):
                image[i] += '........'

        return image

    def print_image(self):
        pass


class Prompter:

    def __init__(self, image=None):
        self.arduino = None
        self.image = Text().image if image is None else image
        self.image_len = len(self.image[0])

        self.time_test = dt.datetime.now()
        self.counter = 0

    def __extract_package(self, frame_nr: int, row: str):
        """
        Get 8 characters to send to arduino from 'ascii' image
        :param frame_nr: Frame number of which will be shown on display
        :param row: Row of the image
        :return: Encoded to UTF-8 list of 8 chars
        """
        package = ''
        for i in range(frame_nr, frame_nr + 8):
            if i + 8 > self.image_len - 1:
                package += row[i - len(row) - 1]
            else:
                package += row[i]
        package += '!'

        return package.encode('UTF-8')

    def __increment_pid(self, old: id):
        """
        Increment index of frame, zero it when at the end of text to prompt
        :param old: Old frame number
        :return: Frame number after incrementing
        """
        if old not in [self.image_len, self.image_len - 1]:
            self.arduino.reset_output_buffer()
            return old + 2
        else:
            return 0

    def run(self, times=None):
        """
        Run loop for sending data to arduino
        :param times: How many times display given text, when None given text will go for infinity
        """
        frame_nr = 0
        if times is None:
            while True:
                self.send_package(frame_nr)
                frame_nr = self.__increment_pid(frame_nr)
        else:
            for i in range(int((times * self.image_len) / 2)):
                self.send_package(frame_nr)
                frame_nr = self.__increment_pid(frame_nr)
                print(self.counter)
                self.counter += 1

    def connect(self, port: str, baudrate=9600):
        """
        Connect to arduino
        :param port: Port where arduino is connected
        :param baudrate: Baudrate of sending data, should be the same as in arduino code
        """
        try:
            self.arduino = serial.Serial(port, baudrate)
            time.sleep(2)
        except serial.SerialException as err:
            print(err)
            print('Please change port to which arduino is connected')

    def send_package(self, frame_nr: int):
        if self.arduino is not None:
            for i in range(8):
                row = self.image[i]
                package = self.__extract_package(frame_nr, row)
                self.arduino.write(package)
                time.sleep(.005)
        else:
            raise serial.SerialException('There was no connection made with arduino, use connect() first')


if __name__ == '__main__':
    prompter = Prompter()
    prompter.connect('COM4', baudrate=38400)
    prompter.run()
