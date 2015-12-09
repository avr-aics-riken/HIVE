function LoadSTL(property)
	local stlloader
	stlloader = require("StlLoader")()
	stlloader:Load(property.filepath)
	return stlloader
end
