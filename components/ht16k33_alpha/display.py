import esphome.codegen as cg
from esphome.const import CONF_ID
from ..ht16k33_base.display import (
    base_to_code,
    CONF_SECONDARY_DISPLAYS,
    CONFIG_SCHEMA_BASE,
    ht16k33_ns,
    HT16K33BaseDisplay,
)
import esphome-config_validation as cv

AUTO_LOAD = ['ht16k33_base']

HT16K33AlphaDisplay = ht16k33_ns.class_("HT16K33AlphaDisplay", HT16K33BaseDisplay)

CONFIG_SCHEMA = CONFIG_SCHEMA_BASE.extend(
    cv.Schema({cv.GenerateID(): cv.declare_id(HT16K337SegmentDisplay)})
)

async def to_code(config):
    instance_var = HT16K33AlphaDisplay.new()
    var = cg.Pvariable(config[CONF_ID], instance_var)
    await base_to_code(var, config)

    if CONF_SECONDARY_DISPLAYS in config:
        for conf in config[CONF_SECONDARY_DISPLAYS]:
            instance_disp = HT16K33AlphaDisplay.new()
            disp = cg.Pvariable(conf[CONF_ID], instance_disp)
            await i2c.register_i2c_device(disp, conf)
            cg.add(var.add_secondary_display(disp))

