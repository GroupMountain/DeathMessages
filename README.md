# DeathMessages
Better Death Messages on MCBE


## Dependencies


### Required Dependency

 `GMLIB`


### Optional Dependency

 `ModAPI`

Loading *ModAPI* enables client-side translation.
That allows players to freely switch languages on their own.

## Death Messages Translation

### Resource Pack Translation
By default, this plugin uses resource packs for translation to ensure perfect compatibility with various addons. <br> 
<br>(The plugin automatically builds the resource pack and sends it to players upon joining. Using resource pack-based translation is recommended.)
>[!IMPORTANT]
>If you wnt to use Resource Pack Translation<br>
>This requires ModAPI to be enabled.<br>
>You can configure language for console in `server.properties` like `language=en_US`
### Server-Side Translation

If you prefer not to use resource packs, you can enable server-side translation in the configuration file.<br> This way, all translations are handled entirely on the server without requiring any client-side resource packs.

>[!IMPORTANT]
>You can configure language for Server-Side Translation in `./plugins/DeathMessages/config/config.json` like `Language=en_US`
#### For issues, please report them via GitHub Issues!!!
#### Pull Requests are also welcome!

You can also customize death messages by editing the language files.<br>
(The language files will be generated in the ./plugins/DeathMessages/lang directory; feel free to modify it as needed.)
>[!WARNING]
> DO NOT modify files in directory `./plugins/DeathMessages/resource`, unless you know what you are doing

High performance: Implemented using native functions with no additional data storage, resulting in minimal performance overhead.
Uses native death message broadcasting, preserving the vanilla showdeathmessages game rule behavior.
Currently restores approximately 95% of Java Edition death messages.

# 开源许可
## 源代码可用性
- 您可以自由地获取、使用和修改本插件的源代码，无论是个人使用还是商业目的。
## 修改发布
- 如果您对本插件进行了修改或衍生创作，并打算分发、发布该修改或衍生作品，您必须开源并且以GPL3.0协议下相同的许可证条件进行分发。
## 版权声明
- 在您分发或发布基于GPL3.0协议的软件时（包括但不限于本插件以及本插件的衍生作品），您必须保留原始版权声明、许可证说明和免责声明。
## 引用链接
- 如果您在一个作品中使用了本插件或者本插件的源码，您需要提供一个明确的引用链接，指向软件的许可证和源代码。
## 对整体的影响
- 如果您将基于本插件与其他插件结合使用，或整合成一个单一的插件，那么整个插件都需要遵守GPL3.0协议进行开源。
