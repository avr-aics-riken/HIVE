function MeshAttToVolume(property)
	local mesh = property.mesh
	local extra = property.extra
	local minX = property.min[1]
	local minY = property.min[2]
	local minZ = property.min[3]
	local maxX = property.max[1]
	local maxY = property.max[2]
	local maxZ = property.max[3]
	local sizeX = property.size[1]
	local sizeY = property.size[2]
	local sizeZ = property.size[3]

	return {
		Volume = function ()
			local ma2vP = MeshAttribToVolume()
			ma2vP:Create(mesh, extra, minX, minY, minZ, maxX, maxY, maxZ)
			ma2vP:ToVolume(sizeX, sizeY, sizeZ)
			local volumedata = ma2vP:VolumeData()
			return volumedata
		end
	}
end
