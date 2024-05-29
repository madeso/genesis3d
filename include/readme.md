# Overview

The design of the Genesis3D API has been an evolutionary process. We have been through several revisions of the APIs, and are constantly striving to refine and polish the APIs that we have to make use of the APIs easier, and more powerful, to make maintenance of the subsystems easier, and to make the APIs more easily extensible.

Great attention has been paid to performance of the APIs, as the engine is designed to be used for realtime 3D rendering, on consumer hardware. Consequently the APIs have to balance efficiency of CPU usage with efficient memory use, along with efficiency of design. We focus on these issues on a daily basis in continuing effort to advance the technology to meet the hardware that is currently in use, as well as the hardware and software needs of the upcoming years.

## Basic Structure and Design Tenets

The Genesis3D API set has been designed around a component model, with a strong emphasis on opaque data structures. Header files are compartementalized, and define only a forward declaration for a type, plus interfaces. As much as is possible we hold to this design, making exceptions where the model imposes an undue burden on the programmer, or would impair efficiency so greatly as to limit the applicability of the API.

The opacity of data structures assists, but does not enforce the maintenance of non-circular data structures. A further design goal for the engine is that the data structures not be self referential. Subsystems include only those definitions that they require to operate, and are designed such that they never need to know about the systems that control them. A properly designed subsystem is something that can be extracted wholesale and used in another context without having to do major surgery to make the subsystem operate in its new context.

We are pushing a container model with reference counts for some of our APIs. There are some naming conventions which can be found throughout the engine, and which will become more prevalent as time goes on.

Basic objects have functions to create and destroy them. There are, in general, three creator functions, and one destroyer. So, you can expect to see the following interfaces on most objects:

    geFoo_Create
    geFoo_CreateRef
    geFoo_CreateFromFile
    geFoo_Destroy

The Create interface is intended for creation of a object from nothing, initialized from a set of parameters passed to the creation function.

The CreateRef interface just bumps the reference count for the object. Internally, we do not care what happens to the object.

CreateFromFile takes a geVFile parameter and creates a Foo object based upon what is parsed from the file.

The Destroy interface decrements the reference count for the object, and if the count reaches zero actually sets about the actual destruction. If the reference count is not zero, the object is still live.

The basic idea is this: There are containers, and objects. You create the objects, via whatever means you choose, and add them to the containers. The container will, in general, call CreateRef on the object, and keep the reference in an internal list. You are free to do whatever you choose with the object you added. If you don't need it anymore, call Destroy. The container's reference to the object will keep the object alive. When the container is destroyed, it will destroy all the objects that it contains.

A correlary to this is that there should be one Destroy call for each and every Create call. If you create a reference, you should destroy the reference. If you create from file, you should destroy the thing you created. If you are a container, and you have added an external object via an Add\* API, you should destroy that object when you are destroyed. From a philisophical standpoint, once you destroy the object, you should assume that the pointer that you hold is invalid, even if you 'know' that you just added it to a container that will keep the object alive for a while.

We have made a design decision in our definition of the CreateRef functions in that these functions return void. This implies that the actual pointer that is passed in as the object to be referenced is a pointer which will be shared by all owners of the object. We chose this model in homage to efficiency; the reference model improves maintainability of the code, but we required that it not introduce signficant overhead to our subsystems.

## Organization Of Components

The engine follows the following structure roughly:

picture lost

In the image above, arrows downward indicate that the system above has knowledge of the system below. Care has been taken in the design of these systems to avoid circularities in the structure.

Probably the easiest way to employ these APIs is to create the topmost containers first in your application (e.g. geEngine), and begin adding sub-containers, and objects incrementally, as you create them. Most of the objects can exist independent of the container objects, but some are acquired directly and solely from their containers (e.g. geEntity).

## What can it do?

The engine is designed primarily to render indoor scenes with moderate polygon count at very high performance. It can be used to build reasonably detailed and extensive outdoor scenes provided that care is taken to build those scenes correctly. The engine has support for fast collision detection, and precalculated lighting and visibility testing.

## What can it not do?

The engine is not designed to render unlimited outdoor terrain currently. It is not designed for fast culling of arbitrary non-sealed geometry. You should not expect to build enormous scenes with high polygon counts and no visibility inhibitors (walls) and get a high frame rate. If you want to build outdoor scenes with the current engine, you should limit the scene size and use Gouraud shading of the outdoor faces to limit texture thrashing. Also, try the new far clipping plane with distance fogging. See the editor documentation and our FAQ sheets for more information on this.
