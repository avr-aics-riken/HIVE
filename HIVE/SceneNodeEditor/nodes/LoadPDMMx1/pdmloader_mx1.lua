function LoadPDMMx1(property)
	local loader = require("PdmLoader")()
	loader:LoadMx1(property.filepath, property.timestep)
	local pointData = loader:PointData(property.container, property.radius)

	--local extraData = pdm:ExtraData('velocity')
	function pointdata()
		return pointData
	end
	return {pointdata=pointdata}
end
