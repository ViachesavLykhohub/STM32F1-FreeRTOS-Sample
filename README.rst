#######################
Abstract-STM32Fx-Sample
#######################

Description
***********

This repository is an example of usage `Abstract-STM32Fx <https://github.com/SlavaLikhohub/Abstract-STM32Fx>` library.

It was tested using the STM32F4DISCOVERY installed in a Global Logic Starter Kit. 
If you are using something else consider reviewing pinouts settings at the beginning of each example.

Requirements
************

List of requirment provided here `Abstract-STM32Fx <https://github.com/SlavaLikhohub/Abstract-STM32Fx>`_.

How to start
************

#. Make sure you have completed the installation steps described in Requirements_ first.
#. Recursively clone the repository:

   .. code-block:: shell-session
      
      git clone --recursive https://github.com/SlavaLikhohub/Abstract-STM32Fx-Sample
      
   or clone first and then initialize all submodules
   
   .. code-block:: shell-session
      
      git clone https://github.com/kpi-keoa/glsk_stm32_toolchain
      git submodule update --init --recursive
      
#. Check pinouts at beginings of **src/*.c** files.
#. Connect your STM32 to computer (NOTE: this example is for the STM32F4).
#. Build and flash the program:
   
   Blinky example

   .. code-block:: shell-session
      
      make PROFILE=release TARGET=blinky tidy target flash

   or LCD example
   
   .. code-block:: shell-session

      make PROFILE=release TARGET=lcd tidy target flash

Documentation
*************

The documentation provided here `Abstract-STM32Fx <https://github.com/SlavaLikhohub/Abstract-STM32Fx>`_.

