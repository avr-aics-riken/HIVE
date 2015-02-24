function Isosurf(property)
	local vm = VolumeToMeshData();
	vm:Create(property.volume, property.isovalue)
	--vm:SetIsovalue(property.isovalue)
	return vm
end
