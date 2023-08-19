import dominant_tones_bindings


class Image:
    def __init__(
        self,
        filename: str = None,
        *,
        raw_data: list[int] = None,
        image: list[list[list[int]]] = None,
        pixels: list[list[int]] = None,
        size: tuple[int, int] = None
    ):
        """
        Initialize an Image object.

        Args:
            filename (str, optional): The filename of the image. Defaults to None.
            raw_data (list[int], optional): The raw image data. Defaults to None.
            image (list[list[list[int]]], optional): The image data in nested list format. Defaults to None.
            pixels (list[list[int]], optional): The pixels of the image. Defaults to None.
            size (tuple[int, int], optional): The size of the image. Defaults to None.

        Raises:
            ValueError: If no valid arguments are provided.
        """
        if filename is not None:
            self._image = dominant_tones_bindings.Image(filename)
        elif raw_data is not None:
            self._image = dominant_tones_bindings.Image(raw_data)
        elif image is not None:
            self._image = dominant_tones_bindings.Image(image)
        elif pixels is not None and size is not None:
            self._image = dominant_tones_bindings.Image(
                pixels, size[0], size[1], True)
        else:
            raise ValueError()

    @property
    def width(self) -> int:
        """
        Get the width of the image.

        Returns:
            int: The width of the image.
        """
        return self._image.width

    @property
    def height(self) -> int:
        """
        Get the height of the image.

        Returns:
            int: The height of the image.
        """
        return self._image.height

    @property
    def channels(self) -> int:
        """
        Get the number of channels in the image.

        Returns:
            int: The number of channels in the image.
        """
        return self._image.channels

    @property
    def size(self) -> int:
        """
        Get the size of the image.

        Returns:
            int: The size of the image.
        """
        return self._image.size

    @property
    def shape(self) -> tuple[int, int, int]:
        """
        Get the shape of the image.

        Returns:
            tuple[int, int, int]: The shape of the image.
        """
        return (self._image.height, self._image.width, self._image.channels)

    def __iter__(self):
        """
        Iterate over the image.

        Returns:
            iterator: An iterator over the image.
        """
        return iter(self._image)

    def __getitem__(self, index):
        """
        Get the pixel at the specified index.

        Args:
            index: The index of the pixel.

        Returns:
            int: The pixel value.
        """
        return self._image[index]

    def __setitem__(self, index, value):
        """
        Set the pixel at the specified index.

        Args:
            index: The index of the pixel.
            value: The new pixel value.
        """
        self._image[index] = value

    def __str__(self) -> str:
        """
        Get a string representation of the image.

        Returns:
            str: A string representation of the image.
        """
        return str(self._image)

    def __len__(self) -> int:
        """
        Get the length of the image.

        Returns:
            int: The length of the image.
        """
        return len(self._image)


class KMeans:
    def __init__(self, n_clusters: int = 10, max_iter: int = 300, tol: float = 1e-4, random_state: int = 0):
        """
        Initializes a KMeans object.

        Args:
            n_clusters (int): The number of clusters to form.
            max_iter (int): The maximum number of iterations.
            tol (float): The relative tolerance with regards to inertia to declare convergence.
            random_state (int): Determines random number generation for centroid initialization.
        """
        self._kmeans = dominant_tones_bindings.KMeans(
            n_clusters, max_iter, tol, random_state)

    def fit(self, image: Image):
        """
        Fits the KMeans model to the given image.

        Args:
            image (Image): The image to fit the model to.
        """
        self._kmeans.fit(image._image)

    @property
    def colors(self) -> list[list[int]]:
        """
        Returns the RGB values of the cluster centers.

        Returns:
            list[list[int]]: A list of RGB values for each cluster center.
        """
        return self._kmeans.colors

    @property
    def labels(self) -> list[int]:
        """
        Returns the labels of each sample.

        Returns:
            list[int]: A list of labels, where each label corresponds to a sample.
        """
        return self._kmeans.labels
