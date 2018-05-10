<template>
    <table class="table is-bordered is-striped is-narrow is-hoverable is-fullwidth">
        <thead>
          <tr>
            <th>进程名</th>
            <th>优先级</th>
            <th>已获得时间片</th>
            <th>已用CPU时间</th>
            <th>所需时间</th>
            <th>进程状态</th>
          </tr>
        </thead>
        <tbody>
          <tr 
            v-for="process in data"
            :key="process.name"
            :class="process.state == 'Running' ? 'is-selected' : ''"
          >
            <td>{{process.name}}</td>
            <td>{{process.priority}}</td>
            <td>{{process.allocateRound}}</td>
            <td>{{process.cpuTime}}</td>
            <td>{{process.runTime}}</td>
            <td :class="process.state == 'Finish' ? 'has-text-danger' : ''">{{process.state}}</td>
          </tr>
        </tbody>
      </table>
</template>

<script>
export default {
  props: ["data"]
};
</script>

<style scoped>

</style>