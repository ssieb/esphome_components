import esphome.codegen as cg
from esphome.components import media_player
import esphome.config_validation as cv

from esphome.const import CONF_ID

CODEOWNERS = ["@ssieb"]

simple_ns = cg.esphome_ns.namespace("simple")
SimpleMedia = simple_ns.class_("SimpleMedia", cg.Component, media_player.MediaPlayer)

CONFIG_SCHEMA = media_player.media_player_schema(SimpleMedia).extend(
    {
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await media_player.register_media_player(var, config)

