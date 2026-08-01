cmake_minimum_required(VERSION 3.13)

# The AnnotationsDBs subdirectory is gone: the annotations project database -
# its JSON parsing and serialization, its converters and its duplicate aware
# merger - was extracted into the ImagesAnnotator-DataDrivers library, which
# the AnnotatorController now builds its database through. Only the image
# loaders, which scan a filesystem directory or a web page for the images to
# annotate, stayed in the project.
add_subdirectory(ImagesLoaders)
