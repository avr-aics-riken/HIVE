function LoadOBJ(property)
	local objloader
	objloader = require("ObjLoader")()
	objloader:Load(property.filepath)
	return objloader
end
