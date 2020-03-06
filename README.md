# AT Instruction Set  Application Suite

## About

This package contains an **AT instruction Set** Application Suite.

This suite contains several significant parts:

- at

- at_task
- *at_device*
- *at_data_structure*
- *at_timeout*
- at_message
- some inspiration ...

Those  *italic* part support for customization, this means if you feel it not very well, you can drop the default template writing by me to make full use of what you already have. Let we do that.

## Status

![Constructing - Half Completed](<https://github.com/Miao-Mico/at/blob/master/hammer.png>)

## Feature

- **Poly-Instantiation**

  This suite can `initialize multiple instances`.

- **Customization**

  You can make this at suite perform under almost `any platform`, `any device stream`, by means of `customized package`.

  You also can get through replacing your `data structure` into the at suite, via `customized package`.

  I have offer a device package of `the windows file stream`, and a data structure package of `list-based stack and list-based queue`.

  And you and i, we have the rest.

- **Integrated Control Structure**

  Control all the at suite functions conveniently by `the at_control structure`.

## Introduction

- ### **at**

  This part will contains these common component of this suite, such as the at instruction generate and send.

- ### at_task

  This part will offer a `micro task os`, it is used to handle the input/output automatically through the many ways you constructed for her.

  Such as handle `multiple level instruction transition`and verify `other various string in the message queue` at the same time, this situation is noticed by me when i use a gsm module.

  This part may be not `necessary`, but i want to try some thing new, this make me feel good.

  Then it is likely that i make a branch that not have this part.

- ### at_device

  This part is designed to perform `the input/output stream of the device`.

  Each i/o stream function will be packed into a package, so if you want to make it suitable for your platform, just need to take out the `Underlying Driver`, then pack it into the `package`.

- ### at_data_structure

  This part will offer `the data structure` which will be applied in the `at_task` or `the message storage`.

  Of course, the data structure also will appear in front of it's caller in role of some `packages`.

- ### at_timeout

  `As the name suggests`, this part will allow you to checking the timer to make sure if `over time`, and saying, oh man, see your watch, i have wait you for a long time, pls, my package.

  Yep, you can also pack the timer in a `package`, to define your timeout.

- ### at_message

  This part will let `the message storage` under the command of `the data structure` you select, combining with the at_data_structure. 

  There have two message storage pool in the at suite:

  - transmit message pool

    I recommend to use `the queue structure`, because it's `FIFO` feature.

    It now appears that the transmit, it works like `an assembly line`.

  - feedback message pool

    About this pool, how about `the stack structure` and it's `FILO` feature?

    It is `more important` that `the message receive now`, it may break out suddenly because your initiative communication, than `the message some much more older`.

## Reference

[stack](https://github.com/Miao-Mico/stack): Another repo of mine. This repo offer data structures.

## FAQ

## License

[Apache-2.0](https://github.com/Miao-Mico/at/blob/master/LICENSE)