function LoadSTL(property)
	local stlloader
	stlloader = STLLoader()
	stlloader:Load(property.filepath)
	return stlloader
end
