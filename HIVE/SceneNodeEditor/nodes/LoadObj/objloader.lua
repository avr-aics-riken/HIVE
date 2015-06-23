function LoadOBJ(property)
	local objloader
	objloader = OBJLoader()
	objloader:Load(property.filepath)
	return objloader
end
