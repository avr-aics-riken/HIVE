function SparseVolumeToVolumeFunc(property)
	local rs = property.resample
	if rs == '' then
		rs = 1.0
	end

	local vm = require("SparseVolumeToVolume")();
	vm:Create(property.sparsevolume, rs)
	function volume()
		return vm:VolumeData()
	end
	return { volume = volume }
end
