function SaveImage(property)
	local saver = ImageSaver()
	saver:Save(property.filepath, property.imageData)
end
