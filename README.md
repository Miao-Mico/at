# AT Instruction Set  Application Suite

## About

This package contains an **AT instruction Set** Application Suite.

This suite contains several significant parts:

- at_task
- *at_device*
- *at_data_structure*
- *at_timeout*
- some inspiration ...

Those  *italic* part support for customization, this means if you feel it not very well, you can drop the default template writing by me to make full use of what you already have. Let we do that.

## Introduction

- ### at_task

  This part will offer a `micro task os`, it is used to handle the input/output automatically through the many ways you constructed for her.

  Such as handle `multiple level instruction transition`and verify `other various string in the message queue` at the same time, this situation is noticed by me when i use a gsm module.

  This part may be not `necessary`, but i want to try some thing new, this make me feel good.

  Then it is likely that i make a branch that not have this part.

- ### at_device

  This part is designed to perform `the input/output stream of the device`.

  Each i/o stream function will be packed into a package, so if you want to make it suitable for your platform, just need to take out the `Underlying Driver`, then pack it into the package.

- ### at_data_structure

  This part will offer `the data structure` which will be applied in the `at_task` or `the message storage`.

  Of course, the data structure also will appear in front of it's caller in role of some packages.

- ### at_timeout

  `As the name suggests`, this part will allow you to checking the timer to make sure if `over time`, and saying, oh man, see your watch, i have wait you for a long time, pls, my package.

  Yep, you can also pack the timer in a `package`, to define your timeout.

## FAQ

## License