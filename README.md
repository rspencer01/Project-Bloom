Project Bloom
=============

Overview
--------

An experimental flower rendering system.

The idea is to be able to render fields of tens of thousands of flowers (or even millions) in real time.  This will be aceived via many, many LODs.

Flowers
-------

Each flower is made up of up to 10 000 polygons.  These are prerendered from a number of different directions onto a texture.  Billboarding is then used to actually display the flower.  When the camera comes close, all the triangles are instead rendered, enhancing detail.

Flowers are found in fields.  A similar process is used to render these, as is described above.

Framework
---------

As of the moment, the framework is based off [Project Magrathea III](https://github.com/rspencer01/Project-Magrathea-V3).  No doubt some improvements shall be made.