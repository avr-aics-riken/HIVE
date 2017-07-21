SaveSPH = {}
setmetatable(SaveSPH, {__index = HiveBaseModule})

SaveSPH.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=SaveSPH})
    this.saver = LoadModule("SphSaver")
    return this
end

function SaveSPH:executeSave()
    local v = self.value
    
    if v.volumedata == nil then
        return false
    end
	self.saver:SetVolumeData(v.volumedata)
    
    if v.only_export then
        if HIVE_EXPORTMODE then
            return self.saver:Save(v.filepath)
        else
            return true
        end
    else
        return self.saver:Save(v.filepath)
    end
end

function SaveSPH:Do()
    self:UpdateValue()
    
    if mpiMode() == true then
        if mpiRank() == 0 then
            return self:executeSave()
        end
    else
        return self:executeSave()
    end
end

