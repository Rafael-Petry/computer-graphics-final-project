# 22-bounding-sphere

Create a Collider class and make BoundingBox extend it. Create a BoundingSphere class that also extends Collider and define a generateBoundingSphere method to ColliderGenerator. Change the Object to instead of having boundingBox, have a collider property for more flexibility. The Spaceship will still have a BoundingBox, while all classes that extend CelestialBody will have a BoundingSphere. Implement in BoundingSphere a test for BoundingSphere agains BoundingSphere and BoundingSphere agains BoundingBox.
