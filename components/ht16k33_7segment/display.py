import esphome.codegen as cg
from esphome.const import CONF_ID
from ..ht16k33_base.display import (
    base_to_code,
    CONF_SECONDARY_DISPLAYS,
    CONFIG_SCHEMA,
    ht16k33_ns,
    HT16K33BaseDisplay,
)

AUTO_LOAD = ['ht16k33_base']

HT16K337SegmentDisplay =  ht16k33_ns.class_("HT16K337SegmentDisplay", HT16K33BaseDisplay)

async def to_code(config):
    instance_var = HT16K337SegmentDisplay.new()
    var = cg.Pvariable(config[CONF_ID], instance_var)
    await base_to_code(var, config)

    if CONF_SECONDARY_DISPLAYS in config:
        for conf in config[CONF_SECONDARY_DISPLAYS]:
            instance_disp = HT16K337SegmentDisplay.new()
            disp = cg.Pvariable(conf[CONF_ID], instance_disp)
            await i2c.register_i2c_device(disp, conf)
            cg.add(var.add_secondary_display(disp))

