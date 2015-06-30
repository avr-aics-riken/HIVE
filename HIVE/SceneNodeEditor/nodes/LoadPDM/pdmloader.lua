function LoadPDM(property)
	local loader = PDMLoader()
	loader:Load(property.filepath, property.timestep)
	local pointData = loader:PointData(property.container, property.radius)

	--local extraData = pdm:ExtraData('velocity')
	function pointdata()
		return pointData
	end
	return {pointdata=pointdata}
end