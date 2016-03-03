function LoadSPH(property)
	local sphloader
	sphloader = require("SphLoader")()
	sphloader:Load(property.filepath)
	return sphloader
end
