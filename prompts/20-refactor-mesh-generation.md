# 20-refactor-mesh-generation

Change the mesh generation logic to make it so all objects have a reference to its Mesh instead of saving it on a cache of RenderHelper. This reference to the mesh needs to be static so it only needs to load once per class and should be set by the return value of RenderHelper::loadObjMesh. You can remove the getMesh method of RenderHelper because it won't be used anymore and adjust the renderModel method to have the mesh as its parameter instead of the meshPath.
