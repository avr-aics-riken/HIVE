function VolumeToVectorFunc(property)
	local vm = VolumeToVector()
	vm:DivideNumber(property.divideNum[1], property.divideNum[2], property.divideNum[3])
	vm:Create(property.srcvolume)
	return vm
end
