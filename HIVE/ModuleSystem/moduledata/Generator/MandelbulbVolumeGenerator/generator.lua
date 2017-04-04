
MandelbulbVolumeGenerator = {}
setmetatable(MandelbulbVolumeGenerator, {__index = HiveBaseModule})

MandelbulbVolumeGenerator.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.gen = require("MandelbulbVolGen")()
    setmetatable(this, {__index=MandelbulbVolumeGenerator})
    return this
end

function MandelbulbVolumeGenerator:Do()
    self:UpdateValue()
    return true
end

function MandelbulbVolumeGenerator:VolumeData()
    local v = self.value
    if self.gen:Generate(v.width, v.height, v.depth, v.component) then
        return self.gen:VolumeData()
    end
    print("Mandelbulb generate failed")
    return nil
end
