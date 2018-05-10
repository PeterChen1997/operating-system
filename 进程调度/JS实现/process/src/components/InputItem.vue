<template>
  <div class="input-item">
    <input type="text" class="input input-name" :id="index" placeholder="进程名" :value="name" @change="changeInput">
    <div class="select">
      <select @change="changeSelect" :value="value">
        <option>1</option>
        <option>2</option>
        <option>3</option>
        <option>4</option>
        <option>5</option>
        <option>6</option>
      </select>
    </div>
  </div>
</template>

<script>
export default {
  props: ["name", "value", "index"],
  methods: {
    changeSelect(e) {
      // 触发父类.sync监听函数，达到父子组件同步值的效果
      this.$emit("update:value", e.target.value);
    },
    changeInput(e) {
      // 触发父类.sync监听函数，达到父子组件同步值的效果
      this.$emit("update:name", e.target.value);
    }
  }
};
</script>
