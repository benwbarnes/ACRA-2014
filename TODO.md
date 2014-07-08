# Things to do
* Currently, all objects use a reference to the same config object. That means that config's  destructor will be called when any of these objects is destroyed. Fix that!

