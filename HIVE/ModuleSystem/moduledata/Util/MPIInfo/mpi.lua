MPIInfo = {}
setmetatable(MPIInfo, {__index = HiveBaseModule})

MPIInfo.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=MPIInfo})
    return this
end

function MPIInfo:Do()
    -- nothing to do
    self:UpdateValue()
    local v = self.value
    return true
end

function MPIInfo:mpimode()
    return mpiMode()
end

function MPIInfo:mpirank()
    return mpiRank()
end

function MPIInfo:mpisize()
    return mpiSize()
end
