# Fiefdom
This is my submission for My First Game Jam: Winter 2017. I've written lots of software before, but I've never really
finished a game. I want this to be my first game that I get finished!

## Outline of the Game
(This is mostly for me so I have a direction to go in)

In Fiefdom, you are a feudal lord with a [fief](https://en.wikipedia.org/wiki/Fief) from your gracious king as payment
 for your service in the most recent war. Like any lord, you want your serfs to be productive, so you have plenty of
 money to expand, conquer, and live lavishly. The way you do this is directly via your serfs. Your serfs can farm, fish,
 and go to church. Since your serfs are your primary mode of income, you must make sure that they are happy enough to
 stay motivated, but not *too* happy as they realize what a terrible situation they're in. A serf's motivation is based
 on the *change* in their happiness on a day-to-day basis.

### Serfs
 * A serf loses one point of happiness per day, as a fixed rate.
 * Happiness has a certain cap; thus, a serf cannot forever gain happiness points.
 * If a serf has consistently lost or not gained happiness over the course of 7 days, their motivation loss triples. 
 During this period, they are depressed. Also, motivation is capped at a hard limit.
 * Depression can strike if a serf's happiness is below a certain threshold (TODO: how much?).
 * To kick a serf out of depression, their happiness must rise a net of 10 points since they became depressed.
 * If they are depressed for long enough (TODO: how long?), they will stop eating and happiness income will be halved.
 * A serf's motivation is tied to their happiness. If a serf has gained happiness that day, then they will gain one 
 motivation point. If they lost more than one point of happiness that day, then they will lose one motivation point.
 * A serf's production is directly tied to their motivation.

Things that give happiness points:
 * Going to church
 * Having extra food
 * Having a day off (not while depressed)

Things that take happiness points:
 * Not having enough food
 * Having a loved one die
 * Generally existing
 * Working

Other important points:
 * Food can't be grown in the winter. Plan ahead by stocking up meats and fish during the warmer seasons.

### Buildings
 * Farms are buildings that produce vegetative food. These cannot be stored for very long and will go bad quickly.
 * Farms have a few phases that they go through before they produce food: sowing, planting, growing, harvesting.
 * Pastures contain animals. They're larger and more expensive but they produce a good supply of meat.
 * Churches provide happiness to up to 20 serfs at a time. However, churches are expensive and have upkeep of a priest.
 * Huts hold up to 5 serfs at a time.
 * If a serf doesn't have a hut, they have a chance to die from wolf attacks each day. If it's wintertime, they also
 have a chance of freezing to death.

## Some huge post-game jam TODOs
Because I'm writing this for a game jam, there are going to be some examples of abhorrent code in this game. I want to
try to fix it after the game jam is over. Here's a few things I've done in the interest of time:
* Refactoring subscenes so that they use a common UI base class
* Re-assignable key layouts 
    * Also using virtual key layouts instead of key characters

# License
ISC License; see LICENSE for full details.