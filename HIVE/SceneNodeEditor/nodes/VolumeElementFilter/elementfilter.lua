function VolumeElementFilterFunc(property)
	local elem = property.element
	if elem == nil then
		elem = 0
	end
	local vm = require("FloatsToFloat")();
	vm:Create(property.srcvolume, elem)
	function volume()
		return vm:VolumeData()
	end
	return { volume = volume }
end
