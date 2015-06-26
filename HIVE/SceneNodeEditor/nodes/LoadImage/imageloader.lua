function LoadImage(property)
	local loader = ImageLoader()
	loader:Load(property.filepath)
	return loader
end
