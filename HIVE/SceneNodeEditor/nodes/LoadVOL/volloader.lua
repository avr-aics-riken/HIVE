function LoadVOL(property)
	local volloader
	volloader = require("VolLoader")()
	volloader:Load(property.filepath)
	return volloader
end
