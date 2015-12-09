function SaveSPH(property)
	local sphSaver = require("SphSaver")()()
	sphSaver:SetVolumeDatta(property.volumedata)
	sphSaver:Save(property.filepath)
end
