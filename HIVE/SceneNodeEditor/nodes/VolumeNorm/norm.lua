function VolumeNormFilterFunc(property)
	local elem = property.element
	if elem == nil then
		elem = 0
	end
	local vf = require("VolumeFilter")();
	vf:Norm(property.srcvolume)
	function volume()
		return vf:VolumeData()
	end
	return { volume = volume }
end
