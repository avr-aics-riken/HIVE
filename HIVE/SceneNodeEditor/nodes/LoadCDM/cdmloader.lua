
function LoadCDM(property)
	local loader = require("CdmLoader")()
	loader:Load(property.filepath)
	return loader
end
